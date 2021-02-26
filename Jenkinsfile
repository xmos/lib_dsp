@Library('xmos_jenkins_shared_library@v0.16.0') _

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
        REPO = 'lib_dsp'
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
              forAllMatch("AN00209_xCORE-200_DSP_Library", "app_*/") { path ->
                runXmake(path)
              }

              dir('AN00209_xCORE-200_DSP_Library') {
                runXdoc('doc')
              }
              dir("${REPO}") {
                runXdoc('doc')
              }
            }
          }
        }
        stage('Build XCOREAI') {
          steps {
            dir("${REPO}") {
              forAllMatch("AN00209_xCORE-200_DSP_Library", "app_*/") { path ->
                runXmake(path, '', 'XCOREAI=1')
                dir(path) {
                  stash name: path.split("/")[-1], includes: 'bin/*xcoreai/*.xe, '
                }
              }

              // Build Tests
              dir('tests/') {
                script {
                  tests = [
                    "test_fft_forward_real",
                    "test_fft_inverse_blank_forward"
                  ]
                  tests.each() {
                    dir(it) {
                      runXmake(".", "", "XCOREAI=1")
                      stash name: it, includes: 'bin/*xcoreai/*.xe, '
                    }
                  }
                }
              }
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

    stage('xcore.ai Verification'){
      agent {
        label 'xcore.ai-explorer'
      }
      environment {
        // '/XMOS/tools' from get_tools.py and rest from tools installers
        TOOLS_PATH = "/XMOS/tools/${params.TOOLS_VERSION}/XMOS/xTIMEcomposer/${params.TOOLS_VERSION}"
      }
      stages{
        stage('Install Dependencies') {
          steps {
            sh '/XMOS/get_tools.py ' + params.TOOLS_VERSION
            installDependencies()
          }
        }
        stage('xrun'){
          steps{
            toolsEnv(TOOLS_PATH) {  // load xmos tools
              forAllMatch("AN00209_xCORE-200_DSP_Library", "app_*/") { path ->
                unstash path.split("/")[-1]
              }

              // Run all the tests
              // app_adaptive - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_adaptive.xe &> app_adaptive_test.txt'
              sh 'cat app_adaptive_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_adaptive_test.txt tests/adaptive_test.expect'

              // app_atan2_hypot - no test
              sh 'xrun --io --id 0 bin/xcoreai/app_atan2_hypot.xe'

              // app_bfp - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_bfp.xe &> app_bfp_test.txt'
              sh 'cat app_bfp_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_bfp_test.txt tests/bfp_test.expect'

              // app_complex - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_complex.xe &> app_complex_test.txt'
              sh 'cat app_complex_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_complex_test.txt tests/complex_test.expect'

              // app_complex_fir - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_complex_fir.xe &> app_complex_fir_test.txt'
              sh 'cat app_complex_fir_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_complex_fir_test.txt tests/complex_fir_test.expect'
              // app_dct - no test
              sh 'xrun --io --id 0 bin/xcoreai/app_dct.xe'

              // app_design - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_design.xe &> app_design_test.txt'
              sh 'cat app_design_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_design_test.txt tests/design_test.expect'

              // app_fft - no test
              sh 'xrun --io --id 0 bin/complex_xcoreai/app_fft_complex_xcoreai.xe'
              sh 'xrun --io --id 0 bin/tworeals_xcoreai/app_fft_tworeals_xcoreai.xe'
              sh 'xrun --io --id 0 bin/complex_int16_buf_xcoreai/app_fft_complex_int16_buf_xcoreai.xe'
              sh 'xrun --io --id 0 bin/tworeals_int16_buf_xcoreai/app_fft_tworeals_int16_buf_xcoreai.xe'

              // app_fft_dif - no test
              sh 'xrun --io --id 0 bin/xcoreai/app_fft_dif.xe'

              // app_fft_double_buf - no test
              sh 'xrun --io --id 0 bin/complex_xcoreai/app_fft_double_buf_complex_xcoreai.xe'
              sh 'xrun --io --id 0 bin/tworeals_xcoreai/app_fft_double_buf_tworeals_xcoreai.xe'
              sh 'xrun --io --id 0 bin/complex_int16_buf_xcoreai/app_fft_double_buf_complex_int16_buf_xcoreai.xe'
              sh 'xrun --io --id 0 bin/tworeals_int16_buf_xcoreai/app_fft_double_buf_tworeals_int16_buf_xcoreai.xe'

              // app_fft_real_single - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_fft_real_single.xe &> app_fft_real_single_test.txt'
              sh 'cat app_fft_real_single_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_fft_real_single_test.txt tests/fft_real_single_test.expect'

              // app_fft_timing - no test
              sh 'xrun --io --id 0 bin/xcoreai/app_fft_timing.xe'

              // app_filters - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_filters.xe &> app_filters_test.txt'
              sh 'cat app_filters_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_filters_test.txt tests/filters_test.expect'

              // app_math - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_math.xe &> app_math_test.txt'
              sh 'cat app_math_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_math_test.txt tests/math_test.expect'

              // app_matrix - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_matrix.xe &> app_matrix_test.txt'
              sh 'cat app_matrix_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_matrix_test.txt tests/matrix_test.expect'

              // app_statistics - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_statistics.xe &> app_statistics_test.txt'
              sh 'cat app_statistics_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_statistics_test.txt tests/statistics_test.expect'

              // app_vector - expect
              sh 'xrun --io --id 0 bin/xcoreai/app_vector.xe &> app_vector_test.txt'
              sh 'cat app_vector_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_vector_test.txt tests/vector_test.expect'

              // app_window_post_fft - expect (test_hann)
              sh 'xrun --io --id 0 bin/xcoreai/app_window_post_fft.xe &> app_window_post_fft_test.txt'
              sh 'cat app_window_post_fft_test.txt && diff --ignore-blank-lines --ignore-trailing-space app_window_post_fft_test.txt tests/hann_test.expect'
            }
          }
        }
      }//stages
      post {
        cleanup {
          cleanWs()
        }
      }
    }// xcore.ai

    stage('Update view files') {
      agent {
        label 'x86_64&&brew'
      }
      when {
        expression { return currentBuild.result == "SUCCESS" }
      }
      steps {
        updateViewfiles()
      }
    }
  }
}
