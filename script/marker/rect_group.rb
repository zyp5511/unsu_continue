require_relative 'rect'
class RectGroup
	attr_accessor :rects
	attr_accessor :inferred_rects
	def initialize 
	end
	def initialize arect, airect
		@rects=[arect]
		@inferred_rects=[airect]
	end
	def include arect
		if @rects.count >0
			return @rects.inject(false){|res,rec|res || (rec.include arect)}
		else 
			return false
		end
	end
	def inferred_include arect
		if @inferred_rects.count>0
			return @inferred_rects.inject(false){|res,rec|res || (rec.include arect)}
		else 
			return false
		end
	end
	def add_rect ar, air
		@rects << ar;
		@inferred_rects << air;
	end
	def aggregate
		ax = inferred_rects.map{|x|x.x}.sort!
		ay = inferred_rects.map{|x|x.y}.sort!
		aw = inferred_rects.map{|x|x.w}.sort!  
		ah = inferred_rects.map{|x|x.h}.sort!  
		med = ->(rules){rules[rules.size/2]}
		medx = med.call(ax)
		medy = med.call(ay)
		medw = med.call(aw)
		medh = med.call(ah)
		Rect.new(-1,0,medx,medy,medw,medh)
	end
	def aggregate table
		if @rects.count > 1 
			loop do 
				sum_delta=0;
				@rects.each{|x| sum_delta += adjust(x,table)}
				
				if sum_delta<0.03 
					break
				end
			end
			
		else 
			return nil
		end
	end
	def adjust node,table
		
	end
		
end
