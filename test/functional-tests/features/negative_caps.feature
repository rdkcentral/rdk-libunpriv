Feature: Negative capability handling

  Scenario: Invalid capability name
    Given a capability configuration with invalid names
    When capabilities are parsed
    Then invalid capabilities should be ignored

