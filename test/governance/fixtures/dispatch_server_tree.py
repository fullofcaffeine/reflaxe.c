"""A bounded Haxe launcher stand-in whose grandchild owns a loopback listener."""
from pathlib import Path
import os
import socket
import subprocess
import sys
import time

root = Path(os.environ["HXC_DISPATCH_TREE"])
role, port = sys.argv[1:]
role = "wrapper" if role == "--wait" else role
(root / role).write_text(str(os.getpid()), encoding="ascii")
child = None
listener = None
if role == "grandchild":
    listener = socket.socket()
    listener.bind(("127.0.0.1", int(port)))
    listener.listen()
    listener.settimeout(0.05)
else:
    child_role = "child" if role == "wrapper" else "grandchild"
    child = subprocess.Popen([sys.executable, __file__, child_role, port])

# Even a deliberately broken baseline leaves through the fixture's own stop
# file. The deadline also bounds the fixture if the test process disappears.
try:
    deadline = time.monotonic() + 20
    while not (root / "stop").exists() and time.monotonic() < deadline:
        if role == "wrapper" and (root / "exit-wrapper").exists():
            print("fixture launcher exited", file=sys.stderr, flush=True)
            sys.exit(9)
        if listener is None:
            time.sleep(0.02)
        else:
            try:
                connection, _ = listener.accept()
            except TimeoutError:
                continue
            connection.close()
    if time.monotonic() >= deadline:
        (root / ("expired-" + role)).touch()
finally:
    if listener:
        listener.close()
    if child and (root / "stop").exists():
        child.wait(timeout=3)
