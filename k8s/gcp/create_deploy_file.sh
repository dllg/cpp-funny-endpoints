#!/bin/sh

set -eux

cat <<EOF
apiVersion: apps/v1
kind: Deployment
metadata:
  name: funny-deployment
spec:
  selector:
    matchLabels:
      app: funny
  replicas: 2 # tells deployment to run 2 pods matching the template
  template:
    metadata:
      labels:
        app: funny
    spec:
      containers:
      - name: funny
        image: gcr.io/${PROJ}/${NAME}
        ports:
        - containerPort: 5000
---
kind: Service
apiVersion: v1
metadata:
  name: funny-service
spec:
  selector:
    app: funny
  ports:
  # Default port used by the image
  - port: 5000
---
apiVersion: networking.k8s.io/v1beta1
kind: Ingress
metadata:
  name: funny-ingress
spec:
  rules:
  - host: ${NGINX_INGRESS_IP}.xip.io
  - http:
      paths:
      - path: /
        backend:
          serviceName: funny-service
          servicePort: 5000

