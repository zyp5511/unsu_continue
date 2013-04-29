require 'RMagick'
require 'fileutils'
require_relative 'record'

src = ARGV[0]
des = ARGV[1]
cvdat = ARGV[2]
lcdat = ARGV[3]
transfn = ARGV[4]
table = LCTransformTable.loadMap(transfn,1006) #hard coded cluster number, should be changed later


def parse_cv_data fname
	IO.foreach(fname).map{|x|x.chomp}.chunk{|l|l.end_with?("jpg")||l.end_with?("png") }.each_slice(2).map do |a|
		[a[0][1], a[1][1].map{|x|Rect.makePureRect(x)}]
	end
end


lcrecords = Hash[Record::seperate_records(src,des,IO.foreach(report)).map{|r|[r.filename, r.rects.map{|r| table.tranform(r)]}}]
cvrecords = Hash[parse_cv_data cvdat]

lcrecords.each do |k,v|

	ori = Magick::Image.read(File.join(src,k).to_s).first
	if cvrecords[k]!=nil
		found = false
		cvrecords[k].each do |cvr|
			vid = v.find{|vr| vr.has_point cvr.x+(cvr.w/2),cvr.y+(cvr.h/2)}
			if vid ==nil
				found = true;
				rdraw = Magick::Draw.new
				rdraw.stroke('yellow']).stroke_width(0.5)
				rdraw.fill("transparent")
				rdraw.cvrangle(cvr.x,cvr.y,cvr.x+cvr.w-1,cvr.y+cvr.h-1)
				rdraw.draw(ori)
			end
		end
		if found
		ori.write(File.join(des,k).to_s)
		end
	else 
		puts "CV records not found for #{k}"
	end
end
