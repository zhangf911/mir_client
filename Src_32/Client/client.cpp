#include "client.h"
#include "opcodes.pb.h"
#include "mir_proto.adl.h"

gce_client::gce_client()
{

}

gce_client::~gce_client()
{

}

void gce_client::start(gce::stackful_actor base_actor)
{
	gce::log::logger_t& lg = base_actor.get_context().get_logger();
	try
	{
		size_t ecount = 10;
		gce::errcode_t ec;

		boost::shared_ptr<tcp_resolver_t::iterator> eitr;
		base_actor->match("init").recv(eitr);

		std::cout << (*eitr)->host_name() << "\t" << (*eitr)->service_name() << std::endl;

		gce::asio::tcp::socket skt(base_actor);
		skt.async_connect(*eitr);
		base_actor->match(gce::asio::tcp::as_conn).recv(ec);
		GCE_VERIFY(!ec).except(ec);

		mir::msg_header hdr;
		size_t hdr_len = adata::size_of(hdr);
		
		adata::zero_copy_buffer zbuf;

		char buff[256];
		std::string str("hello world!");
		msg_login login;
		login.set_account("sky");

		mir::echo_message echo_message;
		echo_message.msg = login.SerializeAsString();

		hdr.size = adata::size_of(echo_message);
		hdr.type = 1;

		zbuf.set_write(buff, 256);
		adata::write(zbuf, hdr);
		adata::write(zbuf, echo_message);


		for (size_t i = 0; i < ecount; ++i)
		{
			skt.async_write(boost::asio::buffer(buff, hdr_len + hdr.size));
			base_actor->match(gce::asio::tcp::as_send).recv(ec);
			GCE_VERIFY(!ec).except(ec);

			skt.async_read(hdr_len + hdr.size);
			gce::message::chunk ch(hdr_len + hdr.size);
			size_t bytes_transferred = 0;
			base_actor->match(gce::asio::tcp::as_recv).recv(ec, ch, bytes_transferred);

			zbuf.set_read(ch.data(), hdr_len + hdr.size);
			std::string echo_str;
			adata::read(zbuf, hdr);
			mir::echo_message echo_message_back;
			adata::read(zbuf, echo_message_back);
			echo_str = echo_message_back.msg;
			if (zbuf.bad())
			{
				break;
			}
			//GCE_VERIFY(str == echo_str);
			login.ParseFromString(echo_str);
			str = login.account();
			std::cout << str << std::endl;
		}

		str.assign("bye");

		zbuf.set_write(buff, 256);
		adata::write(zbuf, hdr);
		adata::write(zbuf, str);
		skt.async_write(boost::asio::buffer(buff, hdr_len + hdr.size));
		base_actor->match(gce::asio::tcp::as_send).recv(ec);
		GCE_VERIFY(!ec).except(ec);
	}
	catch (std::exception& ex)
	{
		GCE_ERROR(lg) << ex.what();
	}
}

void gce_client::run(gce::stackful_actor actor)
{

}

void gce_client::stop()
{

}

