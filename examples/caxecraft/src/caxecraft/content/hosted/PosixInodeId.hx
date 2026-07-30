package caxecraft.content.hosted;

/**
	Models the system header's `ino_t` file identity.

	The package reader compares values only within this exact header-owned
	domain; it never exposes or performs arithmetic on inode numbers. The native
	ABI harness independently checks this exact carrier against `ino_t`.
**/
typedef PosixInodeId = c.UInt64;
