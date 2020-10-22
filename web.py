import os
import time
import logging
import subprocess


def init_logger():
    log_format = "[%(filename)s %(lineno)d %(levelname)s] %(message)s "
    date_format = '%Y-%m-%d  %H:%M:%S %a '
    logging.basicConfig(level=logging.INFO,
                        format=log_format,
                        datefmt=date_format,
                        )


def main():
    init_logger()
    os.chdir("/rt-thread/web/rt-thread-3.1.3")

    logging.info("build before : ")
    os.system("ls -al")

    os.system("emcc --version")

    os.system("emmake make")

    logging.info("build after : ")
    os.system("ls -al")



if __name__ == "__main__":
    main()
