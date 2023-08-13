# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2023 The Guild of Calamitous Intent
# see LICENSE for full details
##############################################

from .messages import *
from .parser import Parser

from importlib.metadata import version # type: ignore

__author__ = "Kevin Walchko"
__license__ = "MIT"
__version__ = version("gcigps")