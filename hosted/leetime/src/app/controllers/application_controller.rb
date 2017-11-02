class ApplicationController < ActionController::Base
  http_basic_authenticate_with name: 'JHTC', password: 'JHTC'
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
end
