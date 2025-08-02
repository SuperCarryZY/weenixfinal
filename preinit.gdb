python import os
python import gdb
set tcp auto-retry on
set tcp connect-timeout unlimited
python
port = "1234"
try:
    port = os.environ['GDB_PORT']
    print("GDB connecting to QEMU through port: {}".format(port))
except KeyError:
    print("GDB_PORT env var not set, check weenix script and preinit.gdb")

gdb.execute("target remote localhost:{}".format(port))
end

python sys.path.append(os.getcwd() + "/python")
