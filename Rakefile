cmake = File.read("CMakeLists.txt")
proj = cmake[/project\(([^\s]+) ?.*\)/, 1]

task :default => :run

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
    sh("./#{proj}") 
  end
end

file './build' => :cmake
file "./build/#{proj}" => :make
