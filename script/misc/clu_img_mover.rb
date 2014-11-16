require 'fileutils'
require 'set'
srcfolder = ARGV[0]
desfolder = ARGV[1]
clulist = ARGV[2]
puts "Collecting imgs from #{ srcfolder } to #{ desfolder } acording to #{clulist} ..."
set = Set.new


IO.foreach(clulist).with_index do|c,i|
	ci = c.to_i;
	subdir = "#{desfolder}/#{ci}".chomp
	if !set.include?(ci)
		begin
			FileUtils.mkdir_p(subdir)
		rescue => e
			print e.message
			print e.backtrace.join("\n")
		end
		set<<ci;
	end;
	FileUtils.cp "#{srcfolder}/#{i+1}.jpg", subdir
end

