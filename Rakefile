task :default do 
  rm_rf './build'
  mkdir_p './build'
  chdir './build' do
    sh('cmake', '..')
    ln_sf(File.realpath('compile_commands.json'), '..')

    sh('make')
    sh('./showsdl')
  end
end
