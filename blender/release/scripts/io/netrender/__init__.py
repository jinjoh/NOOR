# This directory is a Python package.

import model
import operators
import client
import slave
import master
import master_html
import utils
import balancing
import ui

# store temp data in bpy module

import bpy

bpy.data.netrender_jobs = []
bpy.data.netrender_slaves = []
bpy.data.netrender_blacklist = []