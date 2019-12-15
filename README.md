
# Rooset

Rooset is a democratic structured decision system. Work in progress


## Set Up

### Requiremnts

- docker
- docker-compose

### Initial setup of development environment

```
  $ docker-compose up -d
  $ docker-exec lfcore bash
  # init-test-db
```

Re-running `init-test-db` will drop all data and reset the database to the 
development scenario.

### Using the development environment

The relevant part of the host filesystem is mounted on top of the containers,
so you can edit the code on the host, and it will appear in the contianers.


Run the containers

```
  $ docker-compose up
```

Start the core services. These will run every 5 seconds.

```
  $ docker-compose exec lfcore bash
  # core-services
```

Start the frontend server. There is no autorestart.
It is available on the host at `http://localhost:8081'.

```
  $ docker-compose exec lffrontend bash
  # frontend-server
```

There are a number of test users set up. All their passwords are `login`.

Usernames:
  - admin
  - determined_poitras
  - tender_hugle
  - dreamy_almeida
  - thirsty_swirles
  - goofy_heisenberg
  - thirsty_babbage
  - sick_lamarr
  - admiring_sammet
  - compassionate_bose
  - fervent_wright
  - focused_bell
  - elated_meninsky
  - romantic_carson
  - admiring_bartik
  - evil_austin
  - mad_mcnulty
  - insane_poincare
  - jovial_blackwell
  - goofy_khorana
  - kickass_fermat
  - drunk_saha
  - angry_ritchie
  - desperate_easley
