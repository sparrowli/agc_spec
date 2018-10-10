pipeline {
  agent any

  environment {
    LDFLAGS = '/usr/local/opt/openssl/lib'
    CPPFLAGS = '/usr/local/opt/openssl/include'
  }

  stages {
    stage('Build') {
      steps {
        sh '''
          #!/bin/sh
          if [ ! -d ./release ]; then
            mkdir -p release
          fi
          echo "cd release"
          cd release

          echo "cmake generate Makefile"
          /usr/local/bin/cmake -DCMAKE_BUILD_TYPE=Release ..
          echo "make build release version configured by cmake"
          make
        '''
      }
    }
    stage('Test') {
      steps {
        sh '''
          #!/bin/sh
          git submodule update --init --recursive
          echo "cd gtest_spec/release"
          if [ ! -d ./gtest_spec/release ]; then
            mkdir -p ./gtest_spec/release
          fi
          cd gtest_spec/release
          /usr/local/bin/cmake -DCMAKE_BUILD_TYPE=Release ..
          make
          ./test_spec
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
