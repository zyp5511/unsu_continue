require_relative 'transform'


src = ARGV[0]

tc = LCTransformSet.loadAll(src)
tc.simplify.each{|x| puts x}
