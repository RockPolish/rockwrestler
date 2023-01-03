import datetime

if __name__ == "__main__":
    s = "BUILT "+datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S")
    f = open("src9/framework/time.s", "w")
    f.write("@ this file is generated automatically by time.py\n")
    f.write(".text\n")
    f.write(".arm\n")
    f.write(".align 2\n")
    f.write(".global txt_buildtime\n")
    f.write("txt_buildtime: .asciz \"" + s + "\"\n")
    f.close()
