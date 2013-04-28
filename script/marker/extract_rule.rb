require_relative 'transform'

src = ARGV[0]

tc = LCTransformSet.loadAll(src)
tc.simplify(:median,20).each{|x| puts x}
