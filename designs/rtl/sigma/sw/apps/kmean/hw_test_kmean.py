# -*- coding:utf-8 -*-
from __future__ import division

import sys
sys.path.append('../../../../../rtl/udm/sw')

import time

import udm
from udm import *

sys.path.append('..')
import sigma
from sigma import *

def hw_test_kmean(sigma, firmware_filename):
    
    return sigma.hw_test_generic(sigma, "KMEAN", firmware_filename, 0.1, verify_data)
