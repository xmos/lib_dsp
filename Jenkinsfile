@Library('xmos_jenkins_shared_library@v0.14.2') _

getApproval()

pipeline {
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
        }
      }
    }
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
