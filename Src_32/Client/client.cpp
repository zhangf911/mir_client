#include "client.h"

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

		echo_header hdr;
		size_t hdr_len = amsg::size_of(hdr);
		amsg::zero_copy_buffer zbuf;

		char buff[256];
		std::string str("hello world!");
		hdr.size_ = amsg::size_of(str);

		zbuf.set_write(buff, 256);
		amsg::write(zbuf, hdr);
		amsg::write(zbuf, str);

		for (size_t i = 0; i < ecount; ++i)
		{
			skt.async_write(boost::asio::buffer(buff, hdr_len + hdr.size_));
			base_actor->match(gce::asio::tcp::as_send).recv(ec);
			GCE_VERIFY(!ec).except(ec);

			skt.async_read(hdr_len + hdr.size_);
			gce::message::chunk ch(hdr_len + hdr.size_);
			size_t bytes_transferred = 0;
			base_actor->match(gce::asio::tcp::as_recv).recv(ec, ch, bytes_transferred);

			zbuf.set_read(ch.data(), hdr_len + hdr.size_);
			std::string echo_str;
			amsg::read(zbuf, hdr);
			amsg::read(zbuf, echo_str);
			if (zbuf.bad())
			{
				break;
			}
			GCE_VERIFY(str == echo_str);

			std::cout << str << std::endl;
		}

		str.assign("bye");
		hdr.size_ = amsg::size_of(str);

		zbuf.set_write(buff, 256);
		amsg::write(zbuf, hdr);
		amsg::write(zbuf, str);
		skt.async_write(boost::asio::buffer(buff, hdr_len + hdr.size_));
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

