require 'fileutils'
require 'set'
srcfolder = ARGV[0]
desfolder = ARGV[1]
clulist = ARGV[2]
puts "Collecting imgs from #{ srcfolder } to #{ desfolder } acording to #{clulist} ..."
set = Set.new
IO.foreach(clulist).with_index do|c,i|
		subdir = "#{desfolder}#{c}".chomp
	if !set.include?(c)
		FileUtils.mkdir(subdir)
		set<<c;
	end;
	FileUtils.mv "#{srcfolder}/#{i+1}.jpg", subdir
end

