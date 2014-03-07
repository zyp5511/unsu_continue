#create data list for gnuplot use
require 'set'
require 'RMagick'
require 'fileutils'
require_relative 'record'

cvdat = ARGV[0]
lcdat = ARGV[1]
headdat = ARGV[2]
transfn = ARGV[3]

table = LCTransformTable.loadMap(transfn,1006) #hard coded cluster number, should be changed later
orihead = IO.readlines(headdat).map{|x|x.to_i}
len = orihead.count

def parse_cv_data fname
	IO.foreach(fname).map{|x|x.chomp}.chunk{|l|l.end_with?("gif")||l.end_with?("jpg")||l.end_with?("png")||l.end_with?("jpeg") }.each_slice(2).map do |a|
		[a[0][1][0], a[1][1].map{|x|Rect.makePureRect(x)}]
	end
end

cvrecords = Hash[parse_cv_data cvdat]
orilcrecords = Record::seperate_records(nil,nil,IO.foreach(lcdat)).select{|r|r.rects!=nil}

total = 3306.0

(1..len).each do |l|
	cso=0
	osc=0
	inter=0
	head = orihead.take(l).to_set
	lcrecords = Hash[orilcrecords.map{|r|[r.filename, r.rects.select{|x|head.include?(x.type)}]}]
	lcrecords.each do |k,v|
		vv = v.map{|orir|table.transform orir};
		if cvrecords[k]!=nil
			cvrecords[k].each do |cvr|
				vid = vv.select{|vr| vr.has_point cvr.x+(cvr.w/2),cvr.y+(cvr.h/2)}
				if vid.size==0
					# miss found
					cso+=1
				else
					#matched
					vid.each{|x|x.matched=true};
					inter+=1
				end
			end
		else 
			#puts "CV records not found for #{k}"
		end
		osc+= vv.size-vv.select{|x|x.matched}.size; # still problematic, counting rect w/o consideration of overlapping
	end

	#puts "#{l}:\tTrue Positive: #{inter}\tMissing: #{cso}\tFalse Positive: #{osc}"
	puts "#{inter/total}\t#{osc}"
end

