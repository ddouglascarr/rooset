rooset-http-api
===============

rooset-http-api is responsible for the following:
  - Authentication (But not authorization)
  - Transforming HTTP requests into rooset commands
  - Calling the rooset command handler
  - Transforming the command handler response into an Http Response
  - Routing Query requests *


Depencencies
-----

Third party Dependencies:
  - Maven
  - Java 8
  - [http://geteventstore.com](EventStore)
  - Postgres (We use hibernate, so it's easy to configure another RDB)

Project Dependencies:
  - rooset-application-toolkit
  - rooset-declarations
  - rooset-domain-command-posix


Configuration
-------------

Project configuration can be found in src/main/resources/application.yaml

You can [https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-external-config.html](override any of these variables at runtime)


Running Integration Tests
-------------------------

```
  $ mvn test
```

This expects EventStore to be installed at /opt/EventStore. To override:

```
  $ mvn test -Drun.arguments="--rooset.get-event-store.binPath=/path/to/event/store"
```

Running
-------

```
  $ mvn spring-boot:run
```

There is profile named 'clean-dev', which create-drops the user databse, and creates a user at startup. (Username: 'foobar', password:'password1').


```
  $ mvn spring-boot:run -Drun.profiles=clean-dev
```