require_relative 'transform'
require 'set'

fcore_name = "./coreheads.txt"
fhead_name = "./headnodes.txt"
frules= "/home/lichao/research/posecpp/geo/geostd_with_mean.txt"
foutput_name = "rules.txt"

cores= IO.foreach(fcore_name).to_a.map(&:to_i).to_set
heads= IO.foreach(fhead_name).to_a.map(&:to_i).to_set
LCTransformTable.loadTable(frules,heads,cores)


