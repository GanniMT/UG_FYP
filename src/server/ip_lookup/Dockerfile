# For more information, please refer to https://aka.ms/vscode-docker-python
FROM python:3.7-slim

# Keeps Python from generating .pyc files in the container
ENV PYTHONDONTWRITEBYTECODE=1

# Turns off buffering for easier container logging
ENV PYTHONUNBUFFERED=1

WORKDIR /code

# Install pip requirements
COPY requirements.txt requirements.txt
RUN python -m pip install -r requirements.txt

# Expose the HTTP port
EXPOSE 80/tcp

COPY . .

# During debugging, this entry point will be overridden. For more information, please refer to https://aka.ms/vscode-docker-python-debug
CMD ["python", "./lookup.py"]