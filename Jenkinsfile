@Library('xmos_jenkins_shared_library@master') _
getApproval()
pipeline {
  agent {
    label 'x86&&macOS&&Apps'
  }
  environment {
    VIEW = 'dsp'
    REPO = 'lib_dsp'
  }
  options {
    skipDefaultCheckout()
  }
  stages {
    stage('Get view') {
      steps {
        prepareAppsSandbox("${VIEW}", "${REPO}")
      }
    }
    stage('Library checks') {
      steps {
        xcoreLibraryChecks("${REPO}")
      }
    }
    stage('Tests') {
      steps {
        dir("${REPO}/tests/test_biquad") {
          runXwaf('.')
          viewEnv() {
            runPytest()
          }
        }
        dir("${REPO}/tests/dsp_unit_tests") {
          runXwaf('.')
          viewEnv() {
            runPytest()
          }
        }

        runXmostest("${REPO}", 'tests')
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
      cleanWs()
    }
  }
}
