@Library('xmos_jenkins_shared_library@v0.14.2') _

getApproval()

pipeline {
  agent none
  parameters {
    string(
      name: 'TOOLS_VERSION',
      defaultValue: '15.0.2',
      description: 'The tools version to build with (check /projects/tools/ReleasesTools/)'
    )
  }
  stages {
    stage('Standard build and XS2 tests') {
      agent {
        label 'x86_64&&brew'
      }
      environment {
        REPO = 'lib_logging'
        VIEW = getViewName(REPO)
      }
      options {
        skipDefaultCheckout()
      }
      stages {
        stage('Get view') {
          steps {
            xcorePrepareSandbox("${VIEW}", "${REPO}")
          }
        }
        stage('Library checks') {
          steps {
            xcoreLibraryChecks("${REPO}")
          }
        }
        stage('Tests') {
          stages {
            stage('Test Biquad') {
              steps {
                dir("${REPO}/tests/test_biquad") {
                  runWaf('.')
                  viewEnv() {
                    runPytest()
                  }
                }
              }
            }
            stage("Unit tests") {
              steps {
                dir("${REPO}/tests/dsp_unit_tests") {
                  runWaf('.')
                  viewEnv() {
                    runPytest()
                  }
                }
              }
            }
            stage("Legacy Tests") {
              steps {
                runXmostest("${REPO}", 'tests')
              }
            }
          }
        }
        stage('Build') {
          steps {
            dir("${REPO}") {
              /* Cannot call xcoreAppNoteBuild('AN00209_xCORE-200_DSP_Library')
               * due to the use of multiple applications within this app note.
               */
              xcoreAllAppsBuild('AN00209_xCORE-200_DSP_Library')
              dir('AN00209_xCORE-200_DSP_Library') {
                runXdoc('doc')
              }
              dir("${REPO}") {
                runXdoc('doc')
              }
              // Build these individually (or we can extend xcoreAllAppsBuild to support an argument
              dir('AN00209_xCORE-200_DSP_Library/') {
                script {
                  apps = sh(script: 'find . -maxdepth 1 -name app\* | cut -c 3-', returnStdout: true).trim().split("\\r?\\n")
                  apps.each() {
                    dir(it) {
                      runXmake(".", "", "XCOREAI=1")
                      stash name: it, includes: 'bin/*xcoreai/*.xe, '
                    }
                  }
                }
              }
              //dir('AN00209_xCORE-200_DSP_Library/app_adaptive') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_window_post_fft', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_atan2_hypot') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_atan2_hypot', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_bfp') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_bfp', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_complex') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_complex', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_complex_fir') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_complex_fir', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_dct') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_dct', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_design') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_design', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_fft') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_fft', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_fft_dif') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_fft_dif', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_fft_double_buf') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_fft_double_buf', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_fft_real_single') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_fft_real_single', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_fft_timing') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_fft_timing', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_filters') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_filters', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_math') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_math', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_matrix') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_matrix', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_statistics') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_statistics', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_vector') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_vector', includes: 'bin/*xcoreai/*.xe, '
              //}
              //dir('AN00209_xCORE-200_DSP_Library/app_window_post_fft') {
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'app_window_post_fft', includes: 'bin/*xcoreai/*.xe, '
              //}

              //dir('tests/debug_printf_test'){
              //  runXmake(".", "", "XCOREAI=1")
              //  stash name: 'debug_printf_test', includes: 'bin/xcoreai/*.xe'
              //}
            }
          }
        }
      } // stages
      post {
        cleanup {
          xcoreCleanSandbox()
        }
      }
    } // Stage Standard Build


  }
  post {
    success {
      updateViewfiles()
    }
    cleanup {
      xcoreCleanSandbox()
    }
  }
}
