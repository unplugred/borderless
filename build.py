import os
import sys
import shutil
import shlex

os.chdir(os.path.dirname(os.path.abspath(__file__)))

systems = [{
	"name": "Mac",
	"code": "mac",
	"image": "macos-14",
	"compiler": "Xcode",
	"executable": ".app"
},{
	"name": "Win64",
	"code": "win",
	"image": "windows-latest",
	"compiler": "Visual Studio 17 2022",
	"executable": ".exe"
},{
	"name": "Linux",
	"code": "linux",
	"image": "ubuntu-22.04",
	"compiler": "Unix Makefiles",
	"executable": ""
}]
if sys.platform == "darwin":
	system = 0
	import pty
elif sys.platform == "win32" or sys.platform == "cygwin":
	system = 1
	import subprocess
else:
	system = 2
	import pty

def fuzzy_match(term,data):
	lower = str(term).replace(' ','').lower()
	if lower != "":
		for result in data:
			if result.replace(' ','').lower().startswith(lower):
				return result

def debug(string):
	print('\033[1m'+string+'\033[0m')

def alert(string):
	print('\033[1m\033[93m'+string+'\033[0m')

def error(string, exit_code=1):
	print('\033[1m\033[91m'+string+'\033[0m')
	#sys.exit(exit_code)
	sys.exit(1)

def run_command(cmd,ignore_errors=False):
	censored_command = cmd
	#for secret in saved_data["secrets"].values(): TODO
	#	censored_command = censored_command.replace(secret,"***")
	debug("RUNNING COMMAND: "+censored_command)

	if systems[system]["code"] == "win":
		os.environ['SYSTEMD_COLORS'] = '1'
		process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
		while process.poll() == None:
			print(process.stdout.readline().decode("UTF-8"),end='')
		return_code = process.returncode
	else:
		return_code = pty.spawn(shlex.split(cmd), lambda fd: os.read(fd, 1024))

	if return_code != 0:
		if ignore_errors:
			alert("Exited with return code "+str(return_code))
		else:
			error("Exited with return code "+str(return_code)+", exiting...",return_code)

def join(arr):
	return '/'.join(arr)

def copy(path, output):
	debug("COPYING PATH "+path+" TO "+output)

	if not os.path.exists(path):
		error("Invalid path: "+path)
	if output.endswith('/'):
		if not os.path.isdir(os.path.abspath(output+"..")):
			error("Invalid path: "+output)
	elif not os.path.isdir(os.path.dirname(output)):
		error("Invalid path: "+output)

	if os.path.isdir(path):
		if os.path.exists(output):
			shutil.rmtree(output)
		shutil.copytree(path,output)
	else:
		shutil.copy2(path, output)

def prepare():
	debug("PREPARING DEPENDENCIES")
	if systems[system]["code"] == "linux":
		run_command("sudo apt-get update",True)
		# TODO prepare

def configure():
	debug("CONFIGURING")

	cmd = "cmake -B \"build_"+systems[system]["code"]+"\" -G "
	if systems[system]["code"] == "win":
		run_command(cmd+"\""+systems[system]["compiler"]+"\" -T host=x64 -A x64")
	else:
		run_command(cmd+"\""+systems[system]["compiler"]+"\"")

def build(config):
	debug("BUILDING "+config.upper()+" VERSION")
	run_command("cmake --build \"build_"+systems[system]["code"]+"\" --config "+config+" --target borderless")
	copy(join(["build_"+systems[system]["code"],"borderless"+systems[system]["executable"]]),join(["dist","borderless"+systems[system]["executable"]]))
	if systems[system]["code"] == "mac":
		# TODO codesigning
		pass

def run_plugin():
	artefact = join(["dist","borderless"+systems[system]["executable"]])
	if systems[system]["code"] == "mac":
		run_command("open -W \""+artefact+"\"")
	elif systems[system]["code"] == "win":
		run_command("\""+artefact+"\"")
	else:
		run_command("gdb -ex run \""+artefact+"\"")

def build_installer():
	debug("BUILDING INSTALLER")
	# TODO build installer

def run_program(string):
	if string.strip() == "":
		error("You must specify arguments!")
	args = shlex.split(string)

	if "prepare".startswith(args[0]) and ',' not in string and len(args) <= 1:
		prepare()
		return

	if "configure".startswith(args[0]) and ',' not in string and len(args) <= 1:
		configure()
		return

	if "build".startswith(args[0]) and ',' not in string and len(args) <= 3:
		config = "release"
		if len(args) >= 2:
			config = fuzzy_match(args[1],["release","debug"])
			if config == None:
				error("Unknown version: "+args[1])
		run = "yes"
		if len(args) >= 3:
			run = fuzzy_match(args[1],["yes","no"])
			if run == None:
				error("Unknown version: "+args[1])
		build(config)
		if run == "yes":
			run_plugin()
		return

	if "installer".startswith(args[0]) and ',' not in string and len(args) <= 1:
		build_installer()
		return

if __name__ == "__main__":
	run_program(' '.join(shlex.quote(s) for s in (sys.argv[1:])))
