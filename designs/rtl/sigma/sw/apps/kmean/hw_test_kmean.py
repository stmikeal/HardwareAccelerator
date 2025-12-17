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

verify_data = [0, 0, 0, 0]

def hw_test_kmean_software_sqrt(sigma, firmware_filename):
    print("#### KMEAN TEST STARTED ####")
    print("Loading test program...")
    sigma.tile.loadelf(firmware_filename)
    print("Test program written!")
    time.sleep(1)
    print("Data: " + str(sigma.tile.udm.rd32(0x6000)))
    
    test_succ_flag = 0

    if (test_succ_flag):
        print("#### KMEAN TEST PASSED! ####")
    else:
        print("#### KMEAN TEST FAILED! ####")
    
    print("")
    return test_succ_flag

def hw_test_kmean_software_acc(sigma, firmware_filename):
    return sigma.hw_test_generic(sigma, "K-mean with optimized distance", firmware_filename, 1, verify_data)

def hw_test_kmean_software_xif(sigma, firmware_filename):
    return sigma.hw_test_generic(sigma, "K-mean with xif accelerator", firmware_filename, 1, verify_data)

def hw_test_kmean_software_coprocs(sigma, firmware_filename):
    return sigma.hw_test_generic(sigma, "K-mean with co-processor", firmware_filename, 1, verify_data)
