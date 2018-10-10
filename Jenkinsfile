pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh 'echo "cd release"'
        sh 'cd release'
        sh '''
          echo "cmake generate Makefile"
          cmake -DCMAKE_BUILD_TYPE=Release ..
          echo "make build release version configured by cmake"
          make
        '''
      }
    }
    stage('Test') {
      steps {
        sh 'echo "cd gtest_spec"'
        sh '''
          cd gtest_spec/release
        '''
      }
    }
  }

  post {
    always {
      echo 'This will always run'
    }
    success {
      echo 'This will run only if successful'
    }
    failure {
      echo 'This will run only if failed'
    }
    unstable {
      echo 'This will run only if the run was marked as unstable'
    }
    changed {
      echo 'This will run only if the state of the Pipeline has changed'
      echo 'For example, if the Pipeline was previously failing but is now successful'
    }
  }
}
