require 'spec_helper'
require "net/http"
require "uri"

describe "My Module" do
  it 'should return hello, world' do
    nginx_run_server do
      uri = URI.parse("http://#{nginx_host}:#{nginx_port}/my")
      response = Net::HTTP.get_response(uri)
      response.code.should eq '200'
      response.body.should eq 'hello, world'
    end
  end
end
