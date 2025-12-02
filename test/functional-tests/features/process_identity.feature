Feature: L2 test process identity for rdk-libunpriv

  As a consumer of rdk-libunpriv
  I want to invoke the library through a real helper binary
  So that runtime behavior matches production usage

  Scenario: Helper process starts correctly as root
    Given the L2 helper binary is executed
    When the process starts
    Then the process should start as root
    And the process should not fail during initialization

