use std::convert::TryInto;
use std::fs::File;
use std::io::{Read, Write};

struct Person {
    name: String,
    age: u32,
}

impl Person {
    fn new(name: &str, age: u32) -> Self {
        Person {
            name: name.to_string(),
            age,
        }
    }

    fn print(&self, label: &str) {
        println!("{}: name: {}, age: {}", label, self.name, self.age);
    }

    fn save(&self, filename: &str) -> Result<&Self, std::io::Error> {
        let mut file = File::create(filename)?;

        let name_len: u32 = self.name.len().try_into().unwrap();
        file.write(&name_len.to_be_bytes())?;
        file.write(self.name.as_bytes())?;
        file.write(&self.age.to_be_bytes())?;

        Ok(self)
    }

    fn load(filename: &str) -> Result<Person, std::io::Error> {
        let mut file = File::open(filename)?;

        // read first 4 bytes as a name len
        let mut name_len = [0u8; 4];
        file.read_exact(&mut name_len)?;

        let name_len = u32::from_be_bytes(name_len);

        // create variable of type String with size of name_len
        let mut name = vec![0u8; name_len as usize];
        file.read_exact(&mut name)?;
        let name = String::from_utf8(name).unwrap();

        // read next 4 bytes as age
        let mut age_bytes = [0u8; 4];
        file.read_exact(&mut age_bytes)?;

        // create variable of type u32 with size of age_bytes
        let age = u32::from_be_bytes(age_bytes);

        Ok(Person { name, age })
    }
}

fn main() {
    let p = Person::new("Fadi", 24);
    match p.save("person.bin") {
        Ok(p) => p.print("Memory"),
        Err(e) => println!("Failed to save file: {}", e),
    };

    match Person::load("person.bin") {
        Ok(p) => p.print("Loaded"),
        Err(e) => println!("Failed to load file: {}", e),
    }
}
