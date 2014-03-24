require 'RMagick'
require 'fileutils'

require 'rgl/adjacency'
require 'rgl/traversal'
require 'rgl/connected_components'

require_relative 'rect_group'
require_relative 'transform'

class Record
	attr_accessor :rects,:filename
	attr_accessor :groups
	attr_accessor :headset
	@@colors = Hash.new{|h,k|h[k]="\##{Random.rand(16777216).to_i.to_s(16).rjust(6,'0')}"}
	def initialize(src,des,lines)
		@filename = lines[0];
		@vectors = lines[1]
		@dest = des
		@src = src
		if lines.count > 3
			tmp = lines.drop(2).take_while{|x| x.include?(":")&&!x.include?("=>")}
			if tmp != nil
				@rects = tmp.map do |l| 
					begin 
						Rect.makeRect(l) 
					rescue 
						puts lines.inspect
					end
				end
				#@ori = Magick::Image.read(File.join(src,@filename).to_s).first
			end
		end
	end

	def colortab
		@@colors
	end

	def pick_good_set head
		if @rects !=nil
			@headset = @rects.select{|r|head.include? r.type }
		else 
			@headset = [];
		end
	end
	def group_rects_with_graph  net,table
		@groups = Hash.new
		if @headset==nil
			raise "Empty goodset"
		else
			head_node_lookup = Hash.new
			g = RGL::AdjacencyGraph.new
			@headset.each_with_index do |r,i| 
				head_node_lookup[r]=i
				g.add_vertex(i)
			end
			@headset.combination(2).each do |r,s|
				rule = net.query r.type, s.type
				if rule != nil && ((rule.transform_with_type r).diff s)<0.8
					g.add_edge(head_node_lookup[r],head_node_lookup[s])
				end
			end
			g.each_connected_component do |c|
				c.inject(RectGroup.new){|rg,ri|@groups[@headset[ri]]=rg;rg.add_rect @headset[ri],table.transform(@headset[ri]);rg}
			end
		end
	end


	def group_rects  table
		@groups = Hash.new
		if @headset==nil
			raise "Empty goodset"
		else
			@headset.each do |r|
				ir = table.transform r
				if @groups.values.count>0
					g = @groups.values.find{|v|v.inferred_include ir}
				else 
					g = nil
				end
				if g!=nil
					@groups[r]=g
					g.add_rect r, ir;
				else
					@groups[r]=RectGroup.new(r,ir)
				end
			end
		end
	end

	def load_img
		@ori ||= Magick::Image.read(File.join(@src,@filename).to_s).first
	end
	def export
		self.load_img
		@ori.write(File.join(@dest,@filename).to_s)
	end

	def draw_rect(rect,color=@@colors[rect.type],dash=false)
		self.load_img
		rdraw = Magick::Draw.new
		rdraw.text(rect.x,rect.y+10,rect.type.to_s)
		rdraw.stroke(color).stroke_width(0.5)
		if dash
			rdraw.stroke_dasharray(5,5)
		end
		rdraw.fill("transparent")
		rdraw.rectangle(rect.x,rect.y,rect.x+rect.w-1,rect.y+rect.h-1)
		rdraw.draw(@ori)
	end

	def crop_rect(rect)
		self.load_img
		temp = @ori.crop(rect.x,rect.y,rect.w,rect.h,true)
		type = rect.type
		subdir = "#{@dest}/#{type}".chomp
		if !File.directory?(subdir)
			FileUtils.mkdir(subdir)
		end
		temp.write("#{File.join(subdir,File.basename(@filename, File.extname(@filename))).to_s}_#{rect.x}+#{rect.y}+#{rect.w}x#{rect.h}_#{type}#{File.extname(@filename)}")
	end

	def self.seperate_records(src,des,lines)
		lines.map{|x|x.chomp}.chunk{|l|l.end_with?("gif")||l.end_with?("jpg")||l.end_with?("png")||l.end_with?("jpeg") }.each_slice(2).map{|a| Record.new(src,des,a[0][1]+a[1][1])}
	end

end
