class AddUserTable < ActiveRecord::Migration
  def change
    create_table :users do |u|
      u.string :name, uniqueness: true
      u.string :pass
    end
  end
end
