FROM node:20-alpine

WORKDIR /app

COPY app/package.json app/yarn.lock ./

RUN yarn install --frozen-lockfile

COPY app ./

EXPOSE 5173

CMD ["yarn", "dev", "--host", "0.0.0.0", "--port", "5173"]

