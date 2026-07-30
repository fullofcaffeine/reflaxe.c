package caxecraft.content.hosted;

/**
	Models the system header's signed `off_t` byte offset.

	The first hosted package reader admits a 64-bit `off_t`; the strict native
	harness independently rejects a host with another width or signedness.
**/
typedef PosixOffset = c.Int64;
