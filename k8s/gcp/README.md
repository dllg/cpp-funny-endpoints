# Running on Kubernetes [Google](https://cloud.google.com) cluster

Instructions for mac when using kubectl and gcloud to setup the cluster.

## Install gcloud command utility
Follow instructions from [cloud.google.com](https://cloud.google.com/sdk/docs/quickstart)

## Create a project
```bash
export PROJ="foobar"
gcloud projects create $PROJ --name=$PROJ
```

## Create a gke cluster
```bash
export CLUSTER="foobar-cluster"
gcloud config set project $PROJ
gcloud services enable container.googleapis.com
gcloud container clusters create $CLUSTER
```

## Make sure kubectl uses gke cluster
```bash
export REGION="us-central1-c" # change this to your region
gcloud container clusters get-credentials $CLUSTER --region=$REGION --project=$PROJ
```

## Install ingress-nginx
```yaml
kubectl apply -f ingress-nginx.yaml
```

Update firewall rule for gke to include 8443 port
```bash
RULE=$(gcloud compute firewall-rules list | grep tcp:80,tcp:443 | cut -d ' ' -f1)
gcloud compute firewall-rules update $RULE --allow=tcp:8443,tcp:80,tcp:443
```

```yaml
kubectl wait --namespace ingress-nginx \
  --for=condition=ready pod \
  --selector=app.kubernetes.io/component=controller \
  --timeout=120s
```

Run this command a couple of times until you see that the external ip is present:
```yaml
kubectl get service -n ingress-nginx
```

Export the ingress ip which will be used when deploying the application:
```bash
export NGINX_INGRESS_IP=$(kubectl get service -n ingress-nginx -o json | jq -j '.items[0].status.loadBalancer.ingress[0].ip')
```

## Tag and push the local docker image to gcp
```bash
export NAME=cpp-funny-endpoints
gcloud auth configure-docker
docker tag ${NAME} gcr.io/${PROJ}/${NAME}:latest
docker push gcr.io/${PROJ}/${NAME}:latest
```

## Deploy the application to the cluster
```bash
./create_deploy_file.sh | kubectl apply -f -
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
$ curl ${NGINX_INGRESS_IP}/v1/random
{"message":"Chuck Norris Joke: Chuck Norris is recommending his friends buy 2 story homes because zombies can't climb stairs."}
```

## Cleanup
```bash
gcloud container clusters delete $CLUSTER
```

