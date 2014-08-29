require "bundler/gem_tasks"
require 'rake/extensiontask'
require 'rspec/core/rake_task'

Rake::ExtensionTask.new('pocket_sphinx') do |ext|
  ext.lib_dir = "lib/pocket_sphinx"
end

desc "Run the specs"
RSpec::Core::RakeTask.new do |t|
  t.rspec_opts = "-c"
  t.fail_on_error = false
  t.verbose = false
end

task :spec => :compile

task default: %w[spec]
