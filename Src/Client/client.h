#include "CommDef.h"
#include <gce/actor/all.hpp>
#include <gce/asio/all.hpp>

struct echo_header
{
	uint32_t size_;
};

AMSG(echo_header, (v.size_&sfix));

typedef std::basic_string<gce::byte_t, std::char_traits<gce::byte_t>, std::allocator<gce::byte_t> > bytes_t;
typedef boost::asio::ip::tcp::socket tcp_socket_t;
typedef boost::asio::ip::tcp::resolver tcp_resolver_t;

class gce_client
{
public:
	gce_client();

	~gce_client();
public:
	void start(gce::stackful_actor base_actor);

	void run(gce::stackful_actor actor);

	void stop();

};