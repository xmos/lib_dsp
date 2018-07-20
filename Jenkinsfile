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
        xmosLibraryChecks("${REPO}")
      }
    }
    stage('App Notes') {
      steps {
        buildAppNote("${REPO}/AN00209_xCORE-200_DSP_Library")
      }
    }
    stage('Test') {
      steps {
        runXmostest("${REPO}", "tests") 
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
