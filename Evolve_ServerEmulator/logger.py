# -*- encoding:utf-8 -*-

import logging, os


class logger:
    def __init__(self, path, clevel=logging.DEBUG, Flevel=logging.DEBUG):
        self.logger = logging.getLogger(path)
        self.logger.setLevel(logging.DEBUG)
        fmt = logging.Formatter('[%(asctime)s] [%(levelname)s] %(message)s', '%Y-%m-%d %H:%M:%S')
        # 设置CMD日志
        self.sh = logging.StreamHandler()
        self.sh.setFormatter(fmt)
        self.sh.setLevel(clevel)
        # 设置文件日志
        self.fh = logging.FileHandler(path, encoding='utf-8')
        self.fh.setFormatter(fmt)
        self.fh.setLevel(Flevel)
        
        

    def debug(self, message):
        self.logger.addHandler(self.sh)
        self.logger.addHandler(self.fh)
        self.logger.debug(message)
        self.logger.removeHandler(self.sh)
        self.logger.removeHandler(self.fh)

    def info(self, message):
        self.logger.addHandler(self.sh)
        self.logger.addHandler(self.fh)    
        self.logger.info(message)
        self.logger.removeHandler(self.sh)
        self.logger.removeHandler(self.fh)

    def war(self, message):
        self.logger.addHandler(self.sh)
        self.logger.addHandler(self.fh)    
        self.logger.warn(message)
        self.logger.removeHandler(self.sh)
        self.logger.removeHandler(self.fh)

    def error(self, message):
        self.logger.addHandler(self.sh)
        self.logger.addHandler(self.fh)    
        self.logger.error(message)
        self.logger.removeHandler(self.sh)
        self.logger.removeHandler(self.fh)

    def cri(self, message):
        self.logger.addHandler(self.sh)
        self.logger.addHandler(self.fh)    
        self.logger.critical(message)
        self.logger.removeHandler(self.sh)
        self.logger.removeHandler(self.fh)
