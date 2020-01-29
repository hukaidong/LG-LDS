cmake = File.read("CMakeLists.txt")
proj = cmake[/project\(([^\s]+) ?.*\)/, 1]

task :default => :make

task :cmake do
  mkdir_p './build'
  chdir './build' do
    sh('cmake', '..')
    ln_sf(File.realpath('compile_commands.json'), '..')
  end
end

task :make => ['./build'] do
  chdir './build' do 
    sh('make')
  end
end

task :run => ['./build', "./build/#{proj}"] do
  chdir './build' do 
    sh("./#{proj}", ENV["HOME"]+"/AFO_6Hz_Filtered/Trial_landmark_AFO.csv") 
  end
end

file './build' => :cmake
file "./build/#{proj}" => :make
