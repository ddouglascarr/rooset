
# Rooset

Rooset is a democratic structured decision system. Work in progress


## Set Up

### Requiremnts

- Golang 1.10 +
- Node.js 8 LTS +
- struct2ts (go install github.com/OneOfOne/struct2ts)
- Protobuf golang (https://github.com/golang/protobuf)
- Postgres 9.x +
- Redis

### Bash environment

There's a bunch of env variables. source `etc/set-env` to set them. 

This will source a file at `etc/set-secrets-env`. Use this to override default options
and set secret values. The following needs to be set in that file for Github OAuth to work:
- ROOSET_GITHUB_CLIENT_ID
- ROOSET_GITHUB_CLIENT_SECRET
- ROOSET_GITHUB_APP_ID
- ROOSET_GITHUB_APP_PRIVATE_KEY

All commands assume you're in this directory, and that these env variables are set.

```
  $ . etc/set-env
```

### Postgres DBs

Rooset follows the CQRS pattern. 

It needs 2 postgres databases, `rooset_cmd` and `rooset_query`. 

There's a Bash script to set them up.

```
  $ etc/reset-dbs
```

### Frontend

It's all written in typescript. Build it and the server handles the rest.

```
  $ cd pages
  $ npm install
  $ npm run build
```

During development

```
  $ npm run watch
```

### Generate and install

```
  $ go generate ./...
  $ go install ./cmd/rooset
  $ go install ./cmd/roosetdev
```


## Running

There are 2 components, an http server and a projection processor. Both need to be
running for the system to be fully functional.

Run a server on port 12345

```
  $ rooset server
```

Run the projection processor
```
  $ rooset projproc
```


## Development

There are some development tools available, to make manual testing easier.

```
  $ roosetdev help
```


## Docker Setup

Docker will run a fully contained Gitlab container, as well as the lfcontainers.

```
$ docker-compose build
$ docker-compose up


# Run core services manually
$ docker-compose exec lfcore
$ /usr/local/bin/core-services

# Run frontend server manually
$ docker-compose exec lffrontend
$ /usr/local/bin/start-server

```
