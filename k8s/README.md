# Kubernetes

Instructions for mac on how to run the application as a service inside a [kubernetes](https://kubernetes.io/) cluster.

## Install docker and kubectl

### 1. Install docker desktop
Follow the instructions at [docs.docker.com](https://docs.docker.com/docker-for-mac/install/).

***Make sure to change resources in the preferences menu to use 4 CPU's and 8GB RAM.***

### 2. Install kubectl
```bash
brew install kubernetes-cli
```

If you get an error about symlink to kubectl, run the command:
```bash
brew link --overwrite kubernetes-cli
```

## Set up your cluster

See instructions for:

### kubectl
- [kind](./kind/README.md)
- [google cloud](./gcp/README.md)
