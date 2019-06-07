@Library('xmos_jenkins_shared_library@master') _
pipeline {
  agent none
  environment {
    VIEW = 'dsp'
    REPO = 'lib_dsp'
  }
  options {
    skipDefaultCheckout()
  }
  stages {
    stage('Allow Jenkins run') {
      when {
        expression {
          isOrganisation('xmos')
        }
      }
      options {
        timeout(time: 1, unit: 'DAYS')
      }
      input {
        message 'Allow Jenkins to run on this commit?'
        ok 'Yes'
      }
      agent { // Must be declared after input directive
        label 'x86&&macOS&&Apps'
      }
      steps {
        echo 'Allowed to run'
      }
    }
    stage('Get view') {
      agent {
        label 'x86&&macOS&&Apps'
      }
      steps {
        prepareAppsSandbox("${VIEW}", "${REPO}")
      }
    }
    stage('Library checks') {
      agent {
        label 'x86&&macOS&&Apps'
      }
      steps {
        xcoreLibraryChecks("${REPO}")
      }
    }
    stage('Tests') {
      agent {
        label 'x86&&macOS&&Apps'
      }
      steps {
        runXmostest("${REPO}", 'tests')
      }
    }
    stage('Build') {
      agent {
        label 'x86&&macOS&&Apps'
      }
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
      node('x86&&macOS&&Apps') {
        updateViewfiles()
      }
    }
    cleanup {
      node('x86&&macOS&&Apps') {
        cleanWs()
      }
    }
  }
}
