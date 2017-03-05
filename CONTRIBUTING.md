Contributing
=============

Rooset is an open-source project, and is actively seeking contributors. It's currently pretty rough, so hit me up on twitter (@ddouglascarr) if you would like to help, and I'll point you in the right direction. It is my intention to set the project up with an easy and clear path for first-time contributors, but it's not there yet.

The rest of this document describes the mono-repo and how all the modules fit together. For each module there is a README that provides the specifics about it.

Items marked with an asterisk have not been started.


Modules
========



rooset-application-toolkit (RATK)
---------------------------------

RATK is used to transform message schemas, declared in YAML or JSON, into classes in different languages.

It also has libraries to use these message classes in their respective languages.

It is used in Rooset to create the necessary classes for:

  - A CQRS domain command application (Commands, Events, CommandIntegrationTests)
  - An Http api to the CQRS domain command
  - Ethereum Smart Contracts (CommandRequests, Commands, Events, CommandIntegrationTests) *
  - A web client (CommandRequests, QueryRequests, QueryResponses)



rooset-declarations
------------------

YAML declarations of messages. Transformed by RATK into message classes for other modules.

Contains the following delcaration types:

  - commands
  - events
  - exceptions
  - httpCommandRequests
  - queries
  - tests



rooset-domain-command-posix
-----------------------------

A CQRS command executer, (and saga runner * ) that runs on a POSIX system.

  - Contains the system domain logic (duplicated with rooset-domain-command-ethereum)
  - Takes a JSON formatted Command
  - Evaluates Agains the GetEventStore aggregate event streams
  - Persists an Event to GetEventStore if command is successful
  - Listens to the GetEventStore ( or Ethereum * ) event stream,
        handles side-effects, and executes commands *



rooset-domain-command-ethereum *
--------------------------------

A CQRS-like command executer, that runs on the Ethereum Blockchain

  - Contains the system domain logic (duplicated with rooset-domain-command-posix)
  - Takes a CommandRequest and transforms it into a Command.
  - Emits Ethereum Blockchain events if Command is successful.
  - Alters the state of an aggregate struct within the contract.



rooset-domain-query-reducers
----------------------------

Javascript Event reducers (aka projections).

  - Used to produce query state
  - Runs on the GetEventStore projection engine
  - Runs on a javascript client that reads Ethereum blockchain events. *



rooset-http-api
---------------

Http api.

  - Transforms HttpCommandRequests into Commands, and calls rooset-domain-command-posix
  - Transforms HttpQueryRequests into QueryRequest to the GetEventStore projections system



rooset-web-client
-----------------

Single page web application, the UI for the system:

  - Sends CommandRequests and QueryRequests to rooset-http-api ( OR
        rooset-domain-command-ethereum * )