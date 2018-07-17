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
        libraryChecks("${REPO}")
      }
    }
    stage('App Notes') {
      steps {
        buildAllApps("${REPO}/examples")
        buildAllAppNotes("${REPO}/examples")
      }
    }
    stage('Test') {
      steps {
        runXmostest("${REPO}", "tests") // May want to call runPytest() instead, or as well here.
      }
    }
  }
  post {
    always {
      archiveArtifacts artifacts: "${REPO}/**/*.*", fingerprint: true
      cleanWs()
    }
  }
}
