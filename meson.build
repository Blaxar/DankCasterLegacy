project('DankCaster', 'c')

incdir = include_directories('include')
thread_dep = dependency('threads')
gst_dep = dependency('gstreamer-1.0')
cunit_dep = dependency('cunit')

libdkc_sources = files(['src/libdkc/dkc.c',
                        'src/libdkc/dkc_app.c',
                        'src/libdkc/dkc_backend.c',
                        'src/libdkc/dkc_params.c',
                        'src/libdkc/dkc_scene.c',
                        'src/libdkc/dkc_sink.c',
                        'src/libdkc/dkc_source.c',
                        'src/libdkc/backends/dummy/dkc_dummy_backend.c'])

libdkc_gst_sources = files(['src/libdkc/backends/gstreamer-1.0/gstbkn_start.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_create_scene.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_init.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_create_sink.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_wrap_source.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_delete_source.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_create_source.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_update_wrapped_source.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_create_sink_dummy.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_delete_sink.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_stop.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_create_source_v4l2.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_delete_scene.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_create_source_dummy.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_unwrap_source.c',
                            'src/libdkc/backends/gstreamer-1.0/gstbkn_uninit.c'])
                                                              
libdkc = shared_library('libdkc', [libdkc_sources, libdkc_gst_sources],
                                  dependencies: [thread_dep, gst_dep],
                                  include_directories: incdir)

executable('param-unit-test', 'src/test/unit/dkc_param_test.c',
                               dependencies: [thread_dep, gst_dep, cunit_dep],
                               include_directories: incdir, link_with: libdkc)

executable('source-unit-test', 'src/test/unit/dkc_source_test.c',
                                dependencies: [thread_dep, gst_dep, cunit_dep],
                                include_directories: incdir, link_with: libdkc)

executable('scene-unit-test', 'src/test/unit/dkc_scene_test.c',
                               dependencies: [thread_dep, gst_dep, cunit_dep],
                               include_directories: incdir, link_with: libdkc)

executable('sink-unit-test', 'src/test/unit/dkc_scene_test.c',
                             dependencies: [thread_dep, gst_dep, cunit_dep],
                             include_directories: incdir, link_with: libdkc)