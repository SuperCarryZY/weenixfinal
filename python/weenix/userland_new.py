import subprocess
from os import path

# Define the command and arguments
command = [
    "objdump",
    "--headers",
    "--section=.text",
    "user/usr/bin/s5fstest.exec"
]


class NewUserland(gdb.Command):
    def __init__(self):
      super(NewUserland, self).__init__("new-userland", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        directory = 'user/usr/bin/'
        filename = directory + arg + '.exec'
        if not path.exists(filename):
            filename = 'user/bin/' + arg + '.exec'
        if arg == 'init':
            filename = 'user/sbin/init.exec'
        

        command = f"objdump --headers --section='.text' {filename} | grep .text | awk '{{print $4}}'"

        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)

        if result.returncode == 0:
            print("VMA of the .text section:")
            text_section = result.stdout.strip()

            gdb.execute(f"add-symbol-file {filename} 0x{text_section}")
            gdb.execute(f"break main")
        else:
            print("Command failed with error:")
            print(result.stderr)
        
        
NewUserland()
