# Diagnostic contract fixture

This suite compares the typed `CDiagnosticId`/definition registry with the
machine-readable catalog, renders structured records twice, and validates the
schema-critical fields. Its negative probes reject unregistered IDs, absolute
source paths, and severity changes outside each entry's allowed set.

The suite proves the diagnostic core only. Public `hxc` JSON stream framing,
native-tool attachments, command context, and stdout/stderr integration remain
E8.T09.
