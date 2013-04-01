require_relative 'record'
require 'set'

type = ARGV[0]
src = ARGV[1]
des = ARGV[2]
report = ARGV[3]

records = Record::seperate_records(src,des,IO.foreach(report))
puts "there are #{records.count} records"

if type == "if"
	clufn= ARGV[4]
	head = IO.readlines(clufn).map{|x|x.to_i}.to_set
	c = 0;
	records.each do|x|
		goodset = x.rects.select{|r|head.include? r.type }
		if goodset.count > 0
			c+=1
			goodset.each{|r|x.draw_rect r}
			x.export
		end
	end
	puts " #{c} images are selected"
else
	records.each do|x|
		x.rects.each{|r|x.draw_rect r; x.export}
	end
end
