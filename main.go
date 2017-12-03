package main

import (
	"net"
    "fmt"
    "io/ioutil"
	"net/http"
	"time"
)
func httpClient(reqUrl string, timeout int) (client *http.Client, err error) {

	client = &http.Client{
		Transport: &http.Transport{
			Dial: (&net.Dialer{
                Timeout:   time.Duration(timeout) * time.Second,
            }).Dial,
			ResponseHeaderTimeout: time.Second * time.Duration(timeout),
            DisableKeepAlives: true,
		},
	}

	return
}


func httpGet(reqUrl string, timeout int) (statuscode int, body []byte, err error) {

	var (
		response *http.Response
		client   *http.Client
	)

	client, err = httpClient(reqUrl, timeout)

	if err != nil {
		return
	}

	if response, err = client.Get(reqUrl); err != nil {
		return
	} else {
		statuscode = response.StatusCode
		body, err = ioutil.ReadAll(response.Body)
		defer response.Body.Close()
	}
	return
}

func main() {
    fmt.Println(httpGet("http://127.0.0.1:8181",6));
    return
}

