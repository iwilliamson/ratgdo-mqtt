Import("env")
import shutil

env.Replace(PROGNAME="%s" % env['PIOENV'] + "_sV" + env.GetProjectOption("custom_prog_version"))

#############################################################################
def copy_firmware(source, target, env):
	src = env['PROJECT_BUILD_DIR'] + "/" + env['PIOENV'] + "/firmware.bin"
	trg = env['PROJECT_BUILD_DIR'] + "/../../loader/flasher/" + env['PROGNAME'] + ".bin"
	print ("Copying {} to {}".format(src, trg))
	shutil.copy(src,trg)

env.AddPostAction("buildprog", copy_firmware)
