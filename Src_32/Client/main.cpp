#include "CommDef.h"
#include <gce/actor/all.hpp>
#include <gce/asio/all.hpp>
#include "client.h"


int main()
{
	gce::log::asio_logger lgr;
	gce::log::logger_t lg = boost::bind(&gce::log::asio_logger::output, &lgr, _arg1, "");
	gce::attributes attrs;
	attrs.lg_ = lg;

	try
	{
		gce::errcode_t ec;

		gce::context gce_context(attrs);
		gce::threaded_actor client_base = gce::spawn(gce_context);

		size_t cln_count = 1;
		gce_client client;

		gce::asio::tcp::resolver rsv(client_base);
		boost::asio::ip::tcp::resolver::query qry("127.0.0.1", "12345");
		rsv.async_resolve(qry);
		boost::shared_ptr<tcp_resolver_t::iterator> eitr;
		client_base->match(gce::asio::tcp::as_resolve).recv(ec, eitr);
		GCE_VERIFY(!ec).except(ec);

		for (size_t i = 0; i < cln_count; ++i)
		{
			gce::aid_t cln = spawn(client_base, boost::bind(&gce_client::start, &client, _arg1), gce::monitored);
			client_base->send(cln, "init", eitr);
			//client_base->send(cln, "init", ep);
		}

		for (size_t i = 0; i < cln_count; ++i)
		{
			client_base->recv(gce::exit);
		}

		client_base->recv("finish");

		system("pause");
	}
	catch (std::exception& ex)
	{
		GCE_ERROR(lg) << ex.what();
	}
	
	return 0;
}