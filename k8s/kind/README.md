# Running on Kubernetes [kind](https://kind.sigs.k8s.io/) cluster

Instructions are taken from [kind.sigs.k8s.io](https://kind.sigs.k8s.io/docs/user/quick-start/) with minor changes.

The following instructions are for mac.

## Install kind
```bash
brew install kind
```

## Create the cluster
```bash
kind create cluster --config kind-config.yaml
```

## Make sure kubectl uses kind cluster
```bash
CONTEXT="kind-$(kind get clusters | head -n1 | cut -d " " -f1)"
kubectl config use-context ${CONTEXT}
```

## Install ingress-ngnix
```bash
kubectl apply -f ingress-ngnix.yaml
```

## Deploy the application to the cluster
```bash
kubectl apply -f deploy.yaml
```

## Check that the application is running
```bash
$ kubectl get pods
NAME                               READY   STATUS    RESTARTS   AGE
funny-deployment-76975bdb7-gmrf4   1/1     Running   0          2s
funny-deployment-76975bdb7-xvxmd   1/1     Running   0          51m
```
```bash
$ kubectl get deployment
NAME               READY   UP-TO-DATE   AVAILABLE   AGE
funny-deployment   2/2     2            2           52m
```

## Test that it works as expected
```bash
$ curl localhost/v1/advice
{"message":"Visitors are like fish: As much as you might like them, after three days they start to smell."}
```
