Feature: Drop root capabilities using libunpriv

  Background:
    Given a capability configuration file exists

  Scenario: Drop root with 5 allowed and 2 dropped capabilities
    Given the process starts as root
    And capability file specifies 5 allowed and 2 dropped capabilities
    When drop_root_caps is invoked
    Then process should continue as non-root
    And resulting capabilities should exclude dropped ones

  Scenario: Drop root without capability file
    Given no capability configuration file exists
    When drop_root_caps is invoked
    Then process should fail gracefully
