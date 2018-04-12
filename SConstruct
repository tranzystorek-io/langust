# langust project main SConstruct script

AddOption('--dbg',
          action='store_true',
          help='make a release build')
AddOption('--rel',
          action='store_true',
          help='make a release build')

debug_mode = GetOption('dbg')
release_mode = GetOption('rel')

if not debug_mode and not release_mode:
    debug_mode = True
    release_mode = True

envs = {}
default_env = Environment(CPPPATH='#include', CXXFLAGS='-std=c++11',
                          variant_dir='#/build',
                          target_dir='#/bin')

if debug_mode:
    debug_env = default_env.Clone()
    debug_env.Append(CXXFLAGS=['-g', '-Og'])
    debug_env['variant_dir'] += '/debug'
    debug_env['target_dir'] += '/debug'

    envs['debug'] = debug_env

if release_mode:
    release_env = default_env.Clone()
    release_env.Append(CXXFLAGS=['-O3'])
    release_env['variant_dir'] += '/release'
    release_env['target_dir'] += '/release'

    envs['release'] = release_env

builds = {}
for key in sorted(envs.keys()):
    env = envs[key]
    prog = SConscript('src/SConscript', variant_dir=env['variant_dir'],
                      exports={'env': env}, duplicate=0)

    if key == 'debug':
        testenv = env.Clone()
        #testenv['variant_dir'] += '/test'
        testenv['target_dir'] += '/test'

        test = SConscript('test/SConscript', variant_dir=testenv['variant_dir'] + '/test',
                          exports={'env': testenv}, duplicate=0)
        Depends(test, prog)

        builds['test'] = test;

    builds[key] = prog

if 'release' in builds:
    target = builds['release']

    Default(target)

    run_alias = Alias('run', [target], target[0].abspath)
    AlwaysBuild(run_alias)

if 'debug' in builds:
    target = builds['debug']
    test_target = builds['test']

    test_alias = Alias('test', [test_target], test_target[0].abspath)
    AlwaysBuild(test_alias)

    Default(builds['debug'])
